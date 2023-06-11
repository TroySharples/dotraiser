# DotRaiser

Simple C++ parser for the <code>.ray</code> scene representation format that is often used in ray-tracing applications.

## Format

We support a (hopefully sensible) subset of the .ray standard as specified [here](https://www.cs.cmu.edu/afs/cs.cmu.edu/academic/class/15864-s04/www/assignment4/format.html). Notable differences are as follows:

### No Comments

Comments are not supported. For example

```obj
point_light {
    position = (1,3,-2);
    // yellow light.
    colour = (1,1,0);
}
```

this is illegal.

### Semicolons are Required

All lists enclosed in curley braces must end in a semicolon. For example:

```obj
camera {
    position = (0,0,-4);
    viewdir = (0,0,1);
    updir = (0,1,0);
    aspectratio = 1;
}
```

### Supported Objects

For now, we only support polymesh objects loaded in from a <code>.obj</code> file. The path must be either absolute or relative to the run directory.