# Doxygen Documentation

## Examples

Doxygen is configured such that it finds code examples and snippets in the example directory. Simply place additional examples and snippets there and doxygen should find them.



## Updating the config

```bash
doxygen -u Doxyfile.in
```

This command will remove all obsolete configuration options and update the config file with new configurationoptions.



## Updating Assets

The doxygen theme is [doxygen-awesome](https://github.com/jothepro/doxygen-awesome-css).

It offers two different modes: a light and a dark mode.
To be able to switch between those two modes, we need to specify user-defined (or custom) header and footer files.

The header files were generated in a separate directory via:

```bash
doxygen -w html new_header.html new_footer.html new_stylesheet.css YourConfigFile
```

and then copied to this directory and adapted accordingly.
This ensures that the header and footer files are derived from doxygens default header and footer (of your installed version).

I added the following line to the header file to load the doxygen-awesome javascript file:

```html
<script type="text/javascript" src="$relpath^doxygen-awesome-darkmode-toggle.js"></script>
```

And the following script tag at the end of the body in the footer:

```html
<script type="text/javascript">
  $(document).ready(function(){
      toggleButton = document.createElement('doxygen-awesome-dark-mode-toggle')
      toggleButton.title = "Toggle Light/Dark Mode"
      document.getElementById("MSearchBox").parentNode.appendChild(toggleButton)
  })
</script>
```

as described by the doxygen-awesome theme in order to enable dark-mode toggling.

The doxygen-awesome assets are original unaltered files from the repository.
Theme customization is managed via the `simspark.css` stylesheet.
