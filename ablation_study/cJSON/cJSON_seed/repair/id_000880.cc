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
//<ID> 880
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(root, "initial_count", 3.0);

    // step 2: Configure
    cJSON_AddItemReferenceToObject(root, "name_ref", name);
    cJSON_AddNumberToObject(root, "added_num", 7.0);

    // step 3: Operate and Validate
    char *name_str = cJSON_GetStringValue(name);
    double first_char_code = (double)(unsigned char)name_str[0];
    cJSON_AddNumberToObject(root, "first_char_code", first_char_code);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}