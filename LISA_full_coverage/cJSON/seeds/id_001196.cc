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
//<ID> 1196
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json_buf[] = "{ \"alpha\": \"one\", \"beta\": \"two\", \"flag\": false }    ";
    cJSON_Minify(json_buf);
    cJSON *parsed = cJSON_Parse(json_buf);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    cJSON *first = cJSON_CreateString("first");
    cJSON *second = cJSON_CreateString("second");
    cJSON_AddItemToArray(arr, first);
    cJSON_AddItemToArray(arr, second);
    cJSON *falsy = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, falsy);

    // step 3: Operate
    cJSON_DeleteItemFromArray(arr, 1);
    cJSON *item0 = cJSON_GetArrayItem(arr, 0);
    const char *val0 = cJSON_GetStringValue(item0);
    char *out = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);

    // step 4: Validate & Cleanup
    (void)val0;
    cJSON_free(out);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}