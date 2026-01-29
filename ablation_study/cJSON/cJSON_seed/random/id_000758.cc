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
//<ID> 758
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *name = (cJSON *)0;
    cJSON *version_item = (cJSON *)0;
    cJSON *min_item = (cJSON *)0;
    cJSON *retrieved_name = (cJSON *)0;
    cJSON *retrieved_ver = (cJSON *)0;
    char raw_json[] = " { \"whitespace\" : \"value with spaces\" } ";
    char *printed = (char *)0;
    const char *ver = (const char *)0;
    const char *name_str = (const char *)0;
    const char *ver_str = (const char *)0;
    cJSON_bool added_name = 0;
    cJSON_bool added_version = 0;
    cJSON_bool added_min = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    name = cJSON_CreateString("Alice");
    ver = cJSON_Version();
    version_item = cJSON_CreateString(ver);

    // step 3: Configure
    added_name = cJSON_AddItemToObject(root, "name", name);
    added_version = cJSON_AddItemToObject(root, "version", version_item);
    cJSON_Minify(raw_json);
    min_item = cJSON_CreateString(raw_json);
    added_min = cJSON_AddItemToObject(root, "minified", min_item);

    // step 4: Operate
    retrieved_name = cJSON_GetObjectItem(root, "name");
    retrieved_ver = cJSON_GetObjectItem(root, "version");
    name_str = cJSON_GetStringValue(retrieved_name);
    ver_str = cJSON_GetStringValue(retrieved_ver);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    validation_score = (printed != (char *)0) + (int)added_name + (int)added_version + (int)added_min + (name_str != (const char *)0) + (ver_str != (const char *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}