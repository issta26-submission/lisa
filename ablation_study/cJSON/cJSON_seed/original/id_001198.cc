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
//<ID> 1198
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json_buf[] = "{\n  \"alpha\": \"one\",\n  \"beta\": false\n}\n";
    cJSON_Minify(json_buf);
    cJSON *parsed = cJSON_Parse(json_buf);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *s = cJSON_CreateString("transient");
    cJSON_AddItemToArray(arr, s);
    cJSON *f = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, f);

    // step 3: Operate
    cJSON_DeleteItemFromArray(arr, 0);
    cJSON *s2 = cJSON_CreateString("permanent");
    cJSON_AddItemToArray(arr, s2);

    // step 4: Validate & Cleanup
    char *flat = cJSON_PrintUnformatted(root);
    int buf_len = 128;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}