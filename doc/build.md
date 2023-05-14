# build scripts

I use the build scripts to use in emacs `M-x compile` from any directory and it will build


## dir-locals.el

for emacs users make a file called dir-locals.el and put this in 

```emacs-lisp
((nil . ((projectile-project-compilation-cmd . "./build.sh"))))
```

now you can compile from any c source file

# compile_commands

my .clangd file looks like this 
the pp\_file\_not\_found is for weird endian/machine.h error from the esp-idf

```
CompileFlags:
  Add: -Wno-unknown-warning-option
  Remove: [-m*, -f*]

Diagnostics:
  Suppress: [pp_file_not_found]
```

# components

[components example](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/build-system.html#example-component-requirements)

