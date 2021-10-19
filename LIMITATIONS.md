# Limitations

The following limitations are currently present in the library and needs fixing:

- No UAV, RayTracing, Commands, ...
- TextureCopy is only working on full textures
- SRVs don't support MSAA
- Library is not Multithread save
- WIC Image IO is only working for loading texture (missing storing)
- Upload buffer (Stack) is only working on direct command lists
- No good error handling (only exceptions and Boolean) 