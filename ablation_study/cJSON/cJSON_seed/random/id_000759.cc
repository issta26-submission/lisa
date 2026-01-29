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
//<ID> 759
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
    cJSON *name_item = (cJSON *)0;
    cJSON *ver_item = (cJSON *)0;
    cJSON *minified_item = (cJSON *)0;
    const char *version = (const char *)0;
    char json_buf[] = "{ \"unused\":  123 , \"text\" : \" hello \" }";
    char *printed = (char *)0;
    cJSON_bool added_person = 0;
    cJSON_bool added_name = 0;
    cJSON_bool added_ver = 0;
    cJSON_bool added_minified = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    person = cJSON_CreateObject();
    name_item = cJSON_CreateString("Alice");
    version = cJSON_Version();

    // step 3: Configure
    added_person = cJSON_AddItemToObject(root, "person", person);
    added_name = cJSON_AddItemToObject(person, "name", name_item);
    ver_item = cJSON_CreateString(version);
    added_ver = cJSON_AddItemToObject(root, "cjson_version", ver_item);

    // step 4: Operate
    cJSON_Minify(json_buf);
    minified_item = cJSON_CreateString(json_buf);
    added_minified = cJSON_AddItemToObject(root, "minified_json", minified_item);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    validation_score = (printed != (char *)0) + (int)added_person + (int)added_name + (int)added_ver + (int)added_minified;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}