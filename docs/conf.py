# Minimal Sphinx configuration for k_os
project = 'k_os'
author = 'Christoph Schueler'

extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.napoleon',
    'sphinx.ext.viewcode',
]

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

html_theme = 'alabaster'
html_static_path = ['_static']

master_doc = 'index'
