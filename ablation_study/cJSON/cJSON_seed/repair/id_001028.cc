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
//<ID> 1028
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "version", 1.23);
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON *num2 = cJSON_CreateNumber(20.5);
    cJSON_AddItemToArray(items, num1);
    cJSON_AddItemToArray(items, num2);

    // step 2: Configure
    size_t buf_len = 256;
    char *buf = (char *)cJSON_malloc(buf_len);
    memset(buf, 0, buf_len);
    char *flat = cJSON_PrintUnformatted(root);
    size_t flat_len = strlen(flat);
    size_t to_copy = flat_len < (buf_len - 1) ? flat_len : (buf_len - 1);
    memcpy(buf, flat, to_copy);
    buf[to_copy] = '\0';
    cJSON_AddStringToObject(root, "snapshot", buf);
    cJSON_free(flat);

    // step 3: Operate and Validate
    cJSON *got_items = cJSON_GetObjectItem(root, "items");
    cJSON_bool is_array = cJSON_IsArray(got_items);
    cJSON_AddBoolToObject(root, "items_is_array", is_array);
    cJSON *ver = cJSON_GetObjectItem(root, "version");
    double ver_val = cJSON_GetNumberValue(ver);
    cJSON_AddNumberToObject(root, "version_doubled", ver_val * 2.0);

    // step 4: Cleanup
    cJSON_free(buf);
    char *pretty = cJSON_Print(root);
    cJSON_free(pretty);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}