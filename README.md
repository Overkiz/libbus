libbus
=======
Libbus is a library providing non-blocking dbus communication throught server/proxy model.
It supports method call, method return, and signals dbus messages.

Actually, introspection is not fully supported.

Language: C++

Compiling
=========
Depends on dbus-1, and Overkiz/libcore project.

    autoreconf -vif ; ./configure ; make #should suffice.

Testing
=======
    g++ examples/main.cpp -std=c++11 -lCore -lBus -I/usr/include/dbus-1.0 -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include
    ./a.out server ; ./a.out proxy


Contributing
============


Coding Guidelines
-----------------

We use the following [Astyle](http://astyle.sourceforge.net/) configuration:

    --style=allman
    --indent-namespaces
    --indent=spaces=2
    --indent-switches
    --indent-col1-comments
    --break-blocks
    --delete-empty-lines
    --convert-tabs
    --max-code-length=175
    --indent-preproc-define
    --indent-preproc-cond
    --indent-preproc-block
    --break-blocks
    --min-conditional-indent=0
    --unpad-paren

Submit your changes
-------------------

You are welcome to submit your code change as pull request.
