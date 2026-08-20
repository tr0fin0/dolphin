# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'Dolphin'
copyright = '2026, Guilherme Nunes Trofino'
author = 'Guilherme Nunes Trofino'
release = 'v1.4.0'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
    'breathe',
    'exhale',
    'sphinx.ext.duration',
    'sphinx_design',
    'sphinxcontrib.mermaid',
]

breathe_projects = {
    "firmware": "./firmware/api/xml"
}
breathe_default_project = "firmware"

exhale_args = {
    "createTreeView":        True,
    "containmentFolder":     "./firmware/api/rst",
    "doxygenStripFromPath":  "./firmware/api/xml",
    "rootFileName":          "index.rst",
    "rootFileTitle":         "API",
}

primary_domain = 'c'

templates_path = ['_templates']
exclude_patterns = []



# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'sphinx_rtd_theme'
html_static_path = ['_static']
