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
//<ID> 750
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *ver_item = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *retrieved_ver = (cJSON *)0;
    const char *version_str = (const char *)0;
    char *printed = (char *)0;
    cJSON_bool added_version = 0;
    cJSON_bool added_name = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    version_str = cJSON_Version();
    ver_item = cJSON_CreateString(version_str);
    name_item = cJSON_CreateString("example_name");

    // step 3: Configure
    added_version = cJSON_AddItemToObject(root, "version", ver_item);
    added_name = cJSON_AddItemToObject(root, "name", name_item);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);
    retrieved_ver = cJSON_GetObjectItem(root, "version");

    // step 5: Validate
    validation_score = (printed != (char *)0) + (int)added_version + (int)added_name + (retrieved_ver != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}