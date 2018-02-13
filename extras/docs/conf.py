import sys
import os
import shlex
import subprocess

read_the_docs_build = os.environ.get('READTHEDOCS', None) == 'True'

if read_the_docs_build:
    subprocess.call('doxygen', shell=True)

html_sidebars = { '**': ['globaltoc.html', 'relations.html', 'searchbox.html'], }
html_theme = "sphinx_rtd_theme"

extensions = ['breathe', 'sphinx.ext.intersphinx' ]
breathe_projects = { 'Plaquette': 'xml' }
breathe_default_project = "Plaquette"
templates_path = ['_templates']
source_suffix = '.rst'
master_doc = 'index'
project = u'Plaquette'
copyright = u'2015, Plaquette'
author = u'Plaquette'
version = '0.1.5'
release = '0.1'
language = None
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