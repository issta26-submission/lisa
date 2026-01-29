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
//<ID> 300
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *b_true = cJSON_CreateTrue();
    cJSON *b_false = cJSON_CreateFalse();
    cJSON *name = cJSON_CreateString("example");
    cJSON *count = cJSON_CreateNumber(7.0);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_bool printed_ok = 0;
    int is_true_flag = 0;
    cJSON *retrieved_true = NULL;
    cJSON *retrieved_name = NULL;
    char *name_value = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "flag", b_true);
    cJSON_AddItemToObject(root, "other_flag", b_false);
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddItemToObject(root, "count", count);

    // step 3: Operate / Validate
    printed_ok = cJSON_PrintPreallocated(root, buffer, 256, 1);
    retrieved_true = cJSON_GetObjectItem(root, "flag");
    is_true_flag = (int)cJSON_IsBool(retrieved_true);
    retrieved_name = cJSON_GetObjectItem(root, "name");
    name_value = cJSON_GetStringValue(retrieved_name);
    buffer[0] = (char)('0' + (int)printed_ok);
    buffer[1] = (char)('0' + (is_true_flag % 10));
    buffer[2] = name_value ? name_value[0] : 'X';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}