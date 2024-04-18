# Simple Initialization Chain

## Overview

The initialization chain (aka chain of responsibility) design pattern
is simple: statically constructed chain elements link themselves during
the static initialization into a list and the common initialization
step is executed by a "run" operation that calls the "init" function for
all linked chain elements.

There are several problems with simple chain link design:
1. In most cases there is only one chain. 
2. There is a possibility of repeated "run" operations
that can cause undesirable repeated initializations.
3. It is UT unfriendly.
4. If chain links are provided in some modules (e.g. libraries) of
the program and initialization is driven through another one,
it is possible to miss making the "run" operation call.
5. There is no customization on how locking is performed.
6. In most cases the initialization chain feature will be handcrafted
from program to program due to the lack of built-in customization
handles.

All these issues are addressed by this InitChain class(see
InitChain.inl, InitChain.h and InitChainTagged.h):
1. It provides two ways of creating multiple chains: using templates
or putting every chain into a separate namespace.
2. Repeated "run" operation calls are NOPs because elements are
unlinked from the chain before the funcion execution takes place.
There is a top-level "release" operation that prevents all further
operations. There is "release-link" operation to perform release on
any link. In addition, all access to the top-level level operations
is done through an instance of an accessor/runner class.
3. There is an optional per-chain-element "reset" function and
a top-level "reset" operation to support UT. There are several safety
features to allow using reset functionality at normal run-time when needed.
4. There is a per-chain configuration function to specify whether reset
operations are allowed. It has to be defined in a source code file.
The linker will detect both the cases where this configuration function
is missing and the cases where multiple configuration functions are provided.
It provides a natural anchor point where to put the ?run? operation call.
5. The InitChain uses mutexes for locking, the specific mutex type is provided
by a template tag or #define.
6. There is a possibility to derive from the chain element class. Also,
the process will handle cases when chain elements are deleted or created
from inside a call to its link functions. Both "init" and "reset" functions
may return false to prevent further operations. It seems the InitChain
class is flexible enough to be used without additional hand-crafting.

## Intended Use Case and Limitations

This feature is strictly designed for the narrow use case: collect static
objects into the chain and then execute initialization/reset routines.
There are two simple mutexes one protects internal lists, another prevents
simultaneous execution of the "run", "reset", and "release" operations but
this is the limit of its intended functionality.

There is some flexibility beyond the basics:

1. UT support
2. Derivation from chain element
3. Possibility to delete and create chain elements from inside
both "init" and "reset" function calls.
4. Possibility to release (and then delete) a chain element at any moment.
5. Possibility to release all elements and prevent future activites.

## Operations Overview

The init chain is implemented as the InitChain class. The "run? operation
is implemented as InitChain::Run(). The "reset" operation is implemented as
IinitChain::Reset(). The "release" operation is implemented as
InitChain::Release(). The "release-link" operation is implemented
InitChain::Release(InitChain::Link*)
.
The chain element/chain link is implemented as InitChain::Link class,
both the "init" function and the "reset" function are supplied at
construction time. Both return a bool indicating whether further operations
are allowed. The "init" function is stored as InitChain::Link::init_ and the
"reset" function is stored as InitChain::Link::reset_;

Managed components instantiate an (in most cases static) instance(s) of
InitChain::Link class. These instances link themselves into a static 
initialization list when constructed and unlink themselves when destructed.
The "run" operation unlinks the first chain element from the init list,
calls its "init" function, and optionally inserts it into the reset list.

Repeated "run" operation calls are NOPs.

The "reset" operation does the same steps, calls the "reset" function
and optionally inserts the chain element into the init list. Repeated "reset"
operation calls are NOPs.

The "release" operation clears all lists and prevents new links from being
added by dlopen operations, after that all elements could be safely
deleted,

The "release-link" releases a single chain element.

There are several ways to control allowed operations both at the chain
and link levels and both at run- and compile- times. There is a per-chain
configuration function to allow/disable reset operations. It is called once
from the first "run" operation call. If the "reset" operation is disabled,
all link elements will get at most one "init" function call regardless of
any other setting or return value.

A component may guarantee no more than one "init" function call by not
providing the "reset" function. Also, there will be no further "reset" or
"init" function calls if the :init" function returns false, throws an
exception, or deletes the owning chain element. If the "reset? function
returns false, throws an exception, or deletes the owning chain element
there will be no further "init" and "reset" function calls for the chain
element. The "run", "reset", and "release" operations are private and are
accessible from derivations of the accessor/runner class. It does not add
any access restrictions but it reduces coding errors and simplifies code
analysis through additional visibility of the calling points.

## Code Overview

| File | Description |
|------|-------------|
|InitChain.inl | The core of the implementation.|
|InitChain.h | A basic init chain placed in the "simple" namespace.|
|InitChainTagged.h | Templated implementation.|
|test_common | Managed component examples used by tests.|
|test_namespace | An example with two chains one placed in the "even" and another in the "odd" namespaces.|
|test_shared | An example using components provided as shared libraries.|
|test_simple | A simple example using static linking, also tests exceptions and failures. handling.|
|test_tagges | An example with two chains one tagged with the EvenTag and another with the OddTag.|
|test_common/CompA | The chain link is a standalone static object, no "reset" function, demonstrating that the "init" function return value does not matter in this case.|
|test_common/CompB | A singleton example, the chain link object is a static member of the singleton, the singleton is created by the  "init" function and deleted by the "reset" function.|
|test_common/CompC | Another singleton example. Demonstrates derivation from InitChain::Link, passing a class member function as "init"/"reset" functions into the constructor.|
|test_common/CompD | Another singleton demonstrates failure and exception handling.|
|test_common/CompE | An example of derivation from InitChain::Link the derived chain links aree dynamic members of the owning class, demonstrates deletion of the chain link from inside "init"/"reset" functions.|
|Test_common/Recorder | A test utility to records events.|
|test_common/EvenInitChain.h | Basic init chain placed into the "even" namespace|
|test_common/OddInitChain.h | Basic init chain placed into the "odd" namespace|
|test_common/EvenTag | class EvenTag|
|test_common/OddTag | class OddTag|

## Suggested Use of Level Values
There is nothing wrong to select 100 - 9999
as the normal level.

Components that do not know anything about each other
should use 100. Components that require proper sequence
should use 1000, 1010, 1020 ... .

Values in the 0-99 range should be used for early initialization.

Values in 9000-9999 range should be used for late initialization.

The negative values should be reserved for mocks of the unit tests.
