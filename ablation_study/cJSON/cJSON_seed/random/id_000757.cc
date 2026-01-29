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
//<ID> 757
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *extra = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *version_item = (cJSON *)0;
    char *printed = (char *)0;
    char raw_json[] = "  {  \"extra\" : \" some value \" }  ";
    const char *version_str = (const char *)0;
    cJSON_bool added_name = 0;
    cJSON_bool added_version = 0;
    cJSON_bool added_extra = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("ExampleName");
    version_str = cJSON_Version();
    version_item = cJSON_CreateString(version_str);

    // step 3: Configure
    added_name = cJSON_AddItemToObject(root, "name", name_item);
    added_version = cJSON_AddItemToObject(root, "version", version_item);

    // step 4: Operate
    cJSON_Minify(raw_json);
    extra = cJSON_Parse(raw_json);
    added_extra = cJSON_AddItemToObject(root, "extra", extra);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    validation_score = (printed != (char *)0) + (int)added_name + (int)added_version + (int)added_extra;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}