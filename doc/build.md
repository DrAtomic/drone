# build scripts

I use the build scripts to use in emacs `M-x compile` from any directory and it will build


## dir-locals.el

for emacs users make a file called dir-locals.el and put this in 

```emacs-lisp
((nil . ((projectile-project-compilation-cmd . "./build.sh"))))
```

now you can compile from any c source file
