import sys
import os
import shlex
import subprocess

import sphinx_rtd_theme

read_the_docs_build = os.environ.get('READTHEDOCS', None) == 'True'

if read_the_docs_build:
    subprocess.call('doxygen', shell=True)

# This is to make sure we use animated GIFs in HTML but not in LaTeX
# Source: https://stackoverflow.com/questions/45969711/sphinx-doc-how-do-i-render-an-animated-gif-when-building-for-html-but-a-png-wh
from sphinx.builders.html import StandaloneHTMLBuilder
new_supported_image_types = [
    'image/svg+xml',
    'image/gif',
    'image/png',
    'image/jpeg'
]
additional_default_supported_images = list(set(StandaloneHTMLBuilder.supported_image_types) - set(new_supported_image_types))
StandaloneHTMLBuilder.supported_image_types = new_supported_image_types + additional_default_supported_images

html_sidebars = { '**': ['fulltoc.html', 'relations.html', 'searchbox.html'], }

html_logo = "Plaquette-LogoText.png"
html_favicon = "plaquette-favicon.ico"
html_theme = "sphinx_rtd_theme"
html_style = None
html_theme_options = { 'collapse_navigation' : True }
using_rtd_theme = True

html_show_sourcelink = False

html_css_files = ['css/custom.css']

extensions = ['breathe', 'sphinx.ext.intersphinx', 'sphinx_rtd_theme' ]
breathe_projects = { 'Plaquette': 'xml' }
breathe_default_project = "Plaquette"
templates_path = ['_templates']
source_suffix = '.rst'
master_doc = 'index'
project = u'Plaquette'
copyright = u'2015-2023, Plaquette'
author = u'Sofian Audry & Thomas O. Fredericks'
version = '0.5.0'
release = '0.5.0'
language = 'en'
exclude_patterns = ['_build']
pygments_style = 'sphinx'
todo_include_todos = False
html_static_path = ['_static']
htmlhelp_basename = 'Plaquettedoc'
latex_elements = {
}
latex_documents = [
  (master_doc, 'Plaquette.tex', u'Plaquette Documentation',
   u'Plaquette', 'manual'),
]
