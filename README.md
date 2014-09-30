# Object Store in C

## Tests in command line
You can run the following commands for testing:
```
make clean
make build
make test1
make test2
```
Please see [Makefile](Makefile) for what is being executed.

## Acceptance criterias
Given a user named Marshall
When Marshall created a test file "marshall.txt"
And Marshall created file "marshall.txt with group "public"
Then Marshall should have "rwxpv" access to file "marshall.txt"
And members of group "public" should have access "r---v" to file
"marshall.txt"

Given a user named Jessica
And Jessica is not in any group
And a file "marshall.txt" was created by Marshall with group "public"
Then Jessical should have access "r----" to file "marshall.txt"

Given a user named Marshall
And Marshall created a test file "marshall.txt"
And Marshall created file "marshall.txt with group "public"
When a new ACL with "r" only for Marshall is supplied for file
"marshall.txt"
Then Marshall should have "r----" access to file "marshall.txt"
And Marshall is allowed for "r" action on file "marshall.txt"
And Marshall is denied for any action other than "r" on file
"marshall.txt"
