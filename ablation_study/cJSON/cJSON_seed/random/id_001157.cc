#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 1157
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *person = (cJSON *)0;
    cJSON *names = (cJSON *)0;
    cJSON *name0 = (cJSON *)0;
    cJSON *name1 = (cJSON *)0;
    cJSON *raw_meta = (cJSON *)0;
    cJSON *f_person = (cJSON *)0;
    cJSON *f_names = (cJSON *)0;
    cJSON *f_name1 = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool is_name1_string = 0;
    int validation_score = 0;

    // step 2: Setup - create root and nested object
    root = cJSON_CreateObject();
    person = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "person", person);

    // step 3: Configure - create an array of strings and add a raw metadata object
    names = cJSON_CreateArray();
    name0 = cJSON_CreateString("Alice");
    name1 = cJSON_CreateString("Bob");
    cJSON_AddItemToArray(names, name0);
    cJSON_AddItemToArray(names, name1);
    cJSON_AddItemToObject(person, "names", names);
    raw_meta = cJSON_AddRawToObject(person, "meta", "{\"age\":30,\"active\":true}");

    // step 4: Operate - retrieve nested members and check types
    f_person = cJSON_GetObjectItem(root, "person");
    f_names = cJSON_GetObjectItem(f_person, "names");
    f_name1 = cJSON_GetArrayItem(f_names, 1);
    is_name1_string = cJSON_IsString(f_name1);

    // step 5: Validate - serialize and compute a simple validation score
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);
    validation_score = (int)(root != (cJSON *)0) + (int)(f_person != (cJSON *)0) + (int)(f_name1 != (cJSON *)0) + (int)(is_name1_string != 0) + (int)(raw_meta != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - delete root (frees attached items)
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}