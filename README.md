# IT-202 Project / Operating System : Tag file system

## Synopsis
This projects aims to realize a filesystem including a file tagging system. This tag system allows the user to add or remove tags to any file and to seamlessly search through files or tags.

A tag is a keyword used to categorize a file. A file can be associated to multiple tags and a tag can be shared through multiple files.

This project is developed with [FUSE][], [UT-HASH][] and [UT-LIST][]. The first one helps with file system mounting, the others with file and tag management.

## Code Example

With the following file system:
```
/home/login/Pictures
├── a.jpg
└── b.jpg
└── .db
```

and database file:
```
[a.jpg]
foo
bar
[b.jpg]
foo b.jpg
```

This is a possible execution:
```bash
    $~ > mkdir /home/login/Pictures-with-tags
    $~ > tagfs /home/login/Pictures /home/login/Pictures-with-tags
    $~ > ls /home/login/Pictures-with-tags
    < a.jpg
    < b.jpg
    < foo/
    < bar/
    $~ > ls /home/login/Pictures-with-tags/foo/
    < a.jpg
    < b.jpg
    < bar/
    $~ > ls /home/login/Pictures-with-tags/foo/bar/
    < a.jpg
    $~ > ls /home/login/Pictures-with-tags/bar/
    < a.jpg
    < foo/
    $~ > ls /home/login/Pictures-with-tags/bar/foo/
    < a.jpg
    $~ > ls /home/login/Pictures-with-tags/coin/
    < ls: cannot access 'coin': No such file or directory
```

## Motivation

This project is part of the *fourth year* of *computer science engineering* program at *Bordeaux Graduate School of Engineering*, also known as [ENSEIRB-MATMECA][]*.  
It is meant to give the students an overview of how a file system works and its inherent constraints regarding the performances needed for it to work in extreme conditions, such as large amounts of tags and files.

## Installation

Make sure that you have **make** installed on your system. If not, please download it beforehand.  
For this project to work on your environment, download the **/src** and **/bin** directories and the **Makefile** file. The other directories and files are optional.
Once the necessary resources are downloaded, use **make** to build the project. You can then run your executable.

## Tests

Same as before, except that the command to compile the tests is **make test**.

## Useful links

- Project description : http://people.bordeaux.inria.fr/goglin/teaching/ProjetSysteme/tagfs/index.xhtml
- Repport : http://mrihani.vvv.enseirb-matmeca.fr/donnee/OS_Rapport.pdf
- Presentation : http://mrihani.vvv.enseirb-matmeca.fr/donnee/OS_Presentation.pdf

## License

This project is licensed under the GNU LGPL, Version 3.0. See LICENSE for full license text.

## Contributors

The team of students behind this project is exclusively composed of developers:
- Bastien *COEURET* - Bastien.Coeuret@enseirb-matmeca.fr
- [Mohammed RIHANI][] - Mohammed.Rihani@enseirb-matmeca.fr
- Maxence *RONZIÉ* - Maxence.Ronzie@enseirb-matmeca.fr
- [Vincenzo SCALZI][] - Vincenzo.Scalzi@enseirb-matmeca.fr
- Eya *WESLATI* - Eya.Weslati@enseirb-matmeca.fr


[FUSE]: https://github.com/libfuse/libfuse
[UT-HASH]: https://troydhanson.github.io/uthash/userguide.html
[UT-LIST]: https://troydhanson.github.io/uthash/utlist.html
[ENSEIRB-MATMECA]: http://www.enseirb-matmeca.fr
[Bordeaux-INP]: https://www.bordeaux-inp.fr/
[Mohammed RIHANI]: http://mrihani.vvv.enseirb-matmeca.fr
[Vincenzo SCALZI]: http://vcz.fr/
