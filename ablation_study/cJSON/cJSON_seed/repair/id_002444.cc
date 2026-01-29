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
//<ID> 2444
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "threshold", 42.5);
    cJSON_AddItemToObject(root, "flag", cJSON_CreateTrue());
    cJSON *nested = cJSON_CreateObject();
    cJSON_AddNumberToObject(nested, "inner", 7.0);
    cJSON_AddItemToObject(root, "nested", nested);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed && printed[0] ? printed[0] : '\0';

    // step 3: Operate
    cJSON *thr_item = cJSON_GetObjectItem(root, "threshold");
    double thr_val = cJSON_GetNumberValue(thr_item);
    cJSON *flag_item = cJSON_GetObjectItem(root, "flag");
    cJSON_bool flag_true = cJSON_IsTrue(flag_item);
    cJSON *nested_item = cJSON_GetObjectItem(root, "nested");
    cJSON *inner_item = cJSON_GetObjectItem(nested_item, "inner");
    double inner_val = cJSON_GetNumberValue(inner_item);
    buffer[1] = (char)(int)flag_true;
    buffer[2] = (char)(int)thr_val;
    buffer[3] = (char)(int)inner_val;
    char *printed2 = cJSON_PrintUnformatted(nested_item);
    buffer[4] = printed2 && printed2[0] ? printed2[0] : '\0';

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(printed2);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}