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
//<ID> 537
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *fruits[] = { "apple", "banana", "cherry" };
    cJSON *arr = cJSON_CreateStringArray(fruits, 3);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "fruits", arr);
    cJSON *num = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(root, "value", num);
    const char *ver_str = cJSON_Version();
    cJSON *ver_item = cJSON_CreateString(ver_str);
    cJSON_AddItemToObject(root, "cjson_version", ver_item);

    // step 2: Configure
    cJSON *detached = cJSON_DetachItemFromArray(arr, 1); // detach "banana"
    cJSON_AddItemToObject(root, "favorite", detached);
    double numeric_val = cJSON_GetNumberValue(num);
    size_t buf_len = (size_t)numeric_val + 64;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);

    // step 3: Operate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t snaplen = snapshot ? strlen(snapshot) : 0;
    double extracted_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));
    (void)snaplen;
    (void)extracted_val;

    // step 4: Validate & Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}