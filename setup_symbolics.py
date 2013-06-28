from distutils.core import setup

setup( name='rads.symbolics',
       version='0.1',
       description='Rigorous Analysis of Dynamical Systems',
       author='Rafael Frongillo, Jesse Berwald, and Sarah Day',
       author_email='jberwald@gmail.com',
       package_dir={'' : 'src' },
       packages=['rads', 'rads.symbolics',
                 'rads.misc', 'rads.graphs' ]
       )

       
