Application
#####################
#####################

An application.


Developer Info
##############

Choices of tools and libs and paradigms and ...

Source language         :
Dev. IDE                : 
Versioning              : Git
GUI                     : 
Intended platforms      : all (starting with Windows >=7)

Basic coding guidelines :
  modular/interfacing, general, extensible/reducible/feature controlled,
  platform independent, C++1x standard conform, 'C++-clean' as possible,
  version controlled from start (also binary tagging), logging, performance counting,
  strict separation of gui and logic, memory leak detection, exception handling,
  internationally localizable (string type)
  
Compiler/Linker settings:
  Custom SDKs source: cf. CustomSDKPath.props and SDKPath_[...].props
  Idea: when extracting components submodules for re-use in other projects in the future,
  these should migrate there using a similar folder setup. At least you could add further
  CUSTOM_SDK_PATH-like variables without breaking the so far checked state.
  (You just have to make sure, that the other valid paths don't accidentally contain
  similar SDKs, from which the compiler could mistakenly choose - which is unlikely.)
  
  Bin subfolder represents the complete and valid runtime directory (contains one for each build configuration).
  
Git notes:
  Debug symbol files and exe/dll are only checked for release tagged versions (otherwise they are .gitignore'd).
  Clever tactics is to commit them at release stage and delete them again in a follow-up commit.
  
  