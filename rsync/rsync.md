# rsync

## Using ssh

Include the `-e ssh` argument:

```{bash}
rsync -avz -e ssh source/ destination/
```

## Copying specific file extensions with directory structure

```{bash}
rsync -avz --include '*/' --include '*.extension' --exclude '*' source/ destination/
```
