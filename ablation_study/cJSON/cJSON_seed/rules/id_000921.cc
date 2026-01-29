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
//<ID> 921
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *name = NULL;
    cJSON *flag = NULL;
    const char *name_str = NULL;
    cJSON_bool is_flag_true = 0;
    char *buffer = NULL;
    int digit = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name = cJSON_CreateString("example_node");
    cJSON_AddItemToObject(root, "name", name);
    flag = cJSON_CreateTrue();
    cJSON_AddItemReferenceToObject(root, "enabled", flag);

    // step 3: Operate / Validate
    name_str = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    is_flag_true = cJSON_IsTrue(cJSON_GetObjectItem(root, "enabled"));
    digit = (int)is_flag_true + 5;
    buffer = (char *)cJSON_malloc(32);
    memset(buffer, 0, 32);
    buffer[0] = name_str[0];
    buffer[1] = (char)('0' + (digit % 10));

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}