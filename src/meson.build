giglet_sources = [
  'main.c',
  'giglet-window.c',
]

giglet_deps = [
  dependency('gio-2.0', version: '>= 2.50'),
  dependency('gtk+-3.0', version: '>= 3.22'),
]

gnome = import('gnome')

giglet_sources += gnome.compile_resources('giglet-resources',
  'giglet.gresource.xml',
  c_name: 'giglet'
)

executable('giglet', giglet_sources,
  dependencies: giglet_deps,
  install: true,
)
