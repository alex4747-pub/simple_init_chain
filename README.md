# Simple Initialization Chain

This is a very simple component allowing to simply resolve static initialization issues, by linking initialization functions into a statically constructed list. It is implemented statically constructed a "Chain of Responsibility" design pattern for initialization.

The important part is that there are no source code dependencies
between an entity that is running initialization and the components being initialized. 
# Suggested Use
Derive the initialization worker class from the base and create a static instance with the desired level. Components with a lower level are initialized earlier.  Use InitChain::Run() method to execute init actions. Use InitChain::Reset() to do a reset. See, test_common/comp_a.cc and test_simple/test_main.cc, compile and run test_simple.

By default, duplicate initializations are ignored, use the optional constructor parameter to specify multi-init behavior. 

If multiple independent chains are required use a separate class tag for every chain. See, test_tagged/test_main.cc, compile and run test_tagged.

# Suggested Use of Level Values
There is nothing wrong to select 100 - 9999 as the normal
level.

Components that do not know anything about each other
should use 100. Components that require proper sequence
should use 1000, 1010, 1020 ... .

Values in the 0-99 range should be used for early initialization.

Values in 9000-9999 range should be used for late initialization.

The negative values should be reserved for mocks of the
unit tests.

# Suggested Use of Config Map Parameter
The key should identify the component, it could be a simple
component name or a component plus parameter name
(e.g. "acme_lib" or "acme_lib:priority").

Unit testing is the primary target of this feature. 

# Writing Unit Test-Friendly Initialization workers

1. Implement the ResetFunc method, to clean up the environment after the test case run.
2. Use the default setting (false) of the multi-init parameter. To ignore repeated initialization until explicit reset.

