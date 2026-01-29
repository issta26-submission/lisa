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
//<ID> 1003
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *name_list = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *s3 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(name_list, s1);
    cJSON_AddItemToArray(name_list, s2);
    cJSON_AddItemToArray(name_list, s3);
    cJSON_AddItemToObject(config, "names", name_list);
    cJSON_AddNumberToObject(config, "threshold", 7.5);

    // step 2: Configure
    cJSON *mode = cJSON_CreateString("automatic");
    cJSON_AddItemToObject(config, "mode", mode);
    cJSON_AddNumberToObject(root, "version", 2.0);

    // step 3: Operate & Validate
    int list_size = cJSON_GetArraySize(name_list);
    cJSON_AddNumberToObject(root, "count", (double)list_size);
    cJSON *first_item = cJSON_GetArrayItem(name_list, 0);
    char *first_name = cJSON_GetStringValue(first_item);
    cJSON *first_copy = cJSON_CreateString(first_name);
    cJSON_AddItemToObject(root, "first_name_copy", first_copy);
    cJSON *config_copy = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "config_copy", config_copy);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}