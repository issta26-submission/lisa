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
//<ID> 755
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *greet_item = (cJSON *)0;
    cJSON *version_item = (cJSON *)0;
    cJSON *parsed = (cJSON *)0;
    cJSON *retrieved_name = (cJSON *)0;
    cJSON *retrieved_version = (cJSON *)0;
    char *printed = (char *)0;
    const char *version_str = (const char *)0;
    cJSON_bool added_name = 0;
    cJSON_bool added_greet = 0;
    cJSON_bool added_version = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("Alice");
    greet_item = cJSON_CreateString("Hello, world!");
    version_str = cJSON_Version();
    version_item = cJSON_CreateString(version_str);

    // step 3: Configure
    added_name = cJSON_AddItemToObject(root, "name", name_item);
    added_greet = cJSON_AddItemToObject(root, "greeting", greet_item);
    added_version = cJSON_AddItemToObject(root, "lib", version_item);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);
    parsed = cJSON_Parse(printed);

    // step 5: Validate
    retrieved_name = cJSON_GetObjectItem(parsed, "name");
    retrieved_version = cJSON_GetObjectItem(parsed, "lib");
    validation_score = (printed != (char *)0) + (parsed != (cJSON *)0) + (retrieved_name != (cJSON *)0) + (retrieved_version != (cJSON *)0);

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    (void)validation_score;
    (void)added_name;
    (void)added_greet;
    (void)added_version;
    (void)version_str;
    return 66;
}