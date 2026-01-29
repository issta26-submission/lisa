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
//<ID> 1368
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *pi = cJSON_CreateNumber(3.1415926535);
    cJSON *greet = cJSON_CreateString("hello cJSON");
    cJSON *flag = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToObject(root, "pi", pi);
    cJSON_AddItemToObject(root, "greeting", greet);
    cJSON_AddBoolToObject(root, "is_object", cJSON_IsObject(root));
    cJSON_AddItemToObject(root, "flag", flag);

    // step 3: Operate
    cJSON *pi_item = cJSON_GetObjectItem(root, "pi");
    cJSON *greet_item = cJSON_GetObjectItem(root, "greeting");
    double pi_val = cJSON_GetNumberValue(pi_item);
    const char *greet_val = cJSON_GetStringValue(greet_item);
    double doubled = cJSON_SetNumberHelper(pi_item, pi_val * 2.0);
    cJSON *pi_double_item = cJSON_CreateNumber(doubled);
    cJSON_AddItemToObject(root, "pi_double", pi_double_item);

    // step 4: Validate & Cleanup
    cJSON *retrieved_double = cJSON_GetObjectItem(root, "pi_double");
    double final_pi = cJSON_GetNumberValue(retrieved_double);
    cJSON *greeting_copy = cJSON_CreateString(cJSON_GetStringValue(greet_item));
    cJSON_AddItemToObject(root, "greeting_copy", greeting_copy);
    cJSON *final_record = cJSON_CreateNumber(final_pi);
    cJSON_AddItemToObject(root, "final_pi", final_record);
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    cJSON *printed_raw = cJSON_CreateRaw(buffer);
    cJSON_AddItemToObject(root, "printed", printed_raw);
    cJSON_Delete(root);
    cJSON_free(buffer);
    // API sequence test completed successfully
    return 66;
}