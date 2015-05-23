

## General ##
#### Q. What are the goals of the serecs project? ####
A. Abstractly, the serecs projects's goals are ease of use, fast development, and simple implementations.  More specifically, the serecs project aims to put a server-side ecmascript implementation on par with php, in a php-ish way.

#### Q. Why not use an already existing framework for server-side ecmascript like v8cgi or node.js? ####
A. Node.js is a great framework and is implemented in a really efficient way.  But node.js was designed more for applications rather than websites.  v8cgi is aimed more towards web developers, but suffers from the lack of an easy way to use templates.  The serecs project on the other hands puts templates first and foremost, so it's guaranteed to be fast.

#### Q. From where does the name 'serecs' come? ####
A. SERver ECmaScript.

#### Q. Why is it referred to as the 'serecs project' instead of the 'serecs language', or just 'serecs'? ####
A. The serecs project isn't only a server-side ecmascript - it's the serecs templating language, the serecs api, and the serecs plugin framework.  To merely state it as the serecs language or just serecs undermines the other aspects of the serecs project.

## Design ##

#### Q. Why is the serecs project making somethings php-ish? ####
A. PHP is a powerful (though flawed) language, and has there are a lot of developers who like to use php.  By making the serecs project use similar conventions and functions as PHP, it makes it easier for developers to use their existing skill sets to develop applications easier.

#### Q. Won't adapting php conventions 'muddle' the serecs project? ####
A. While a PHP-ish approach is being taken, the developers are evaluating the pros and cons of each feature deciding whether or not there is a better way to implement the feature, all while trying to maintain a logical and coherent api.  No convention/feature of PHPwill make its way into the serecs project without careful consideration.

### CommonJS ###

#### Q. Why isn't CommonJS a core goal of the serecs project? ####
A. While CommonJS has very nice goals and has many supporters, it is neither complete nor widely enough implemented to be considered a standard, nor does it align with all of the serecs project's goals.  Additionaly, there is still internal debate about parts of the specification, which would make it difficult to implement.

#### Q. Do you plan on implementing any of the CommonJS specification? ####
A. Yes but with some slight modications.  Given that the serecs project attempts to be PHP-ish, the decision to include top level functions such as include() and require() means that the serecs project will not use require() to include a CommonJS modules.  Instead, the serecs project will most likely use modules.require() or commonjs.require() as its method of commonjs module inclusion (although for compatability, the actual modules themselves will be able to use require() according to the commonjs specification).  As an additional note, the CommonJS modules will be implemented as script modules rather than being implemented into the core project.

#### Q. When do you plan on implementing the CommonJS specification? ####
A. Not until development of the first version of the serecs project is complete and out of beta. In short, not until version 1.1 or beyond.