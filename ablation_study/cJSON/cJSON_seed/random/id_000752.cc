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
//<ID> 752
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
    cJSON *retrieved_name_item = (cJSON *)0;
    char *printed = (char *)0;
    const char *retrieved_name = (const char *)0;
    const char *version_str = (const char *)0;
    cJSON_bool added_flag = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("Alice");

    // step 3: Configure
    added_flag = cJSON_AddItemToObject(root, "name", name_item);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);
    version_str = cJSON_Version();
    retrieved_name_item = cJSON_GetObjectItem(root, "name");
    retrieved_name = cJSON_GetStringValue(retrieved_name_item);

    // step 5: Validate
    validation_score = (printed != (char *)0) + (int)added_flag + (retrieved_name != (const char *)0) + (version_str != (const char *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}