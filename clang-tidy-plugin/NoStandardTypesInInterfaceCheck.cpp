#include "clang-tidy/ClangTidy.h"
#include "clang-tidy/ClangTidyCheck.h"
#include "clang-tidy/ClangTidyModule.h"
#include "clang-tidy/ClangTidyModuleRegistry.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"

using namespace clang;
using namespace clang::tidy;
using namespace clang::ast_matchers;

namespace {

class NoStandardTypesInInterfaceCheck : public ClangTidyCheck {
public:
    NoStandardTypesInInterfaceCheck(StringRef Name, ClangTidyContext *Context)
        : ClangTidyCheck(Name, Context) {}

    void registerMatchers(MatchFinder *Finder) override {
        // Match virtual method declarations inside class/struct definitions
        Finder->addMatcher(
            cxxMethodDecl(isVirtual()).bind("method"),
            this);
    }

    void check(const MatchFinder::MatchResult &Result) override {
        const auto *Method = Result.Nodes.getNodeAs<CXXMethodDecl>("method");
        if (!Method)
            return;

        // Check each parameter type
        for (const auto *Param : Method->parameters()) {
            QualType ParamType = Param->getType();
            // Desugar through typedefs (e.g. uint32_t -> unsigned int)
            QualType CanonicalType = ParamType.getCanonicalType();

            if (CanonicalType->isBuiltinType()) {
                diag(Param->getTypeSpecStartLoc(),
                     "parameter '%0' has standard type '%1' in virtual interface; "
                     "consider using a strong type instead")
                    << Param->getName()
                    << ParamType.getAsString();
            }
        }

        // Check return type (but not void)
        QualType ReturnType = Method->getReturnType();
        QualType CanonicalReturnType = ReturnType.getCanonicalType();
        if (CanonicalReturnType->isBuiltinType() && !CanonicalReturnType->isVoidType()) {
            diag(Method->getReturnTypeSourceRange().getBegin(),
                 "virtual interface method '%0' returns standard type '%1'; "
                 "consider using a strong type instead")
                << Method->getName()
                << ReturnType.getAsString();
        }
    }
};

class NoStandardTypesInInterfaceModule : public ClangTidyModule {
public:
    void addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {
        CheckFactories.registerCheck<NoStandardTypesInInterfaceCheck>(
            "custom-no-standard-types-in-interface");
    }
};

} // anonymous namespace

static ClangTidyModuleRegistry::Add<NoStandardTypesInInterfaceModule>
    X("custom-no-standard-types-module",
      "Adds check for standard types in virtual interfaces.");

// This anchor is used to force the linker to link in the generated object file
// and thus register the module.
volatile int NoStandardTypesInInterfaceModuleAnchorSource = 0;
