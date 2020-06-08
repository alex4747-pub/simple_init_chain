# Simple Initialization Chain

This is very simple component allowing to simply resolve static
initialization issues, by linking initialization functions into
a statically constructed list.

It also provides a basis to construct a proper singleton.

# Suggested Use of Level Values

There is nothing wrong to select 100 - 9999 as the normal
level.

Components that to do not know anything about each other
should use 100. Components that require proper sequence
should use 1000, 1010, 1020 ... .

Values in the 0-99 range should be used for early initialization.

Values in 9000-9999 range should be used for late initialization.

The negative values should be reserved for mocks of the
unit tests.

There is a lot of space still left to be filled in,
but it is advisable not to be very inventive.

# Suggested Use of Config Map Parameter

The key should identify the component, it could be a simple
component name or a component plus parameter name
(e.g. "acme_lib" or "acme_lib:priority").

Unit testing is the primary target of this feature.

It could be used to initialize top level application-specific
components

It seems that it should  not be used by generic libraries
due to possible name collisions.

# Writing Unit Test Friendly Components

1. Implement reset method.
2. Skip initialization if mock component is already initialized.
   E.g. singleton initialization can check whether mock
   singleton is already initialized.






