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
//<ID> 1321
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"arr\":[1,2],\"num\":3}";
    cJSON *parsed = cJSON_Parse(json_text);

    // step 2: Configure
    cJSON *arr = cJSON_GetObjectItem(parsed, "arr");
    cJSON *newnum = cJSON_CreateNumber(99.0);
    cJSON_InsertItemInArray(arr, 1, newnum);
    cJSON_SetNumberHelper(newnum, 3.1415);
    cJSON *standalone = cJSON_CreateNumber(777.0);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "standalone", standalone);
    cJSON_AddItemReferenceToArray(arr, standalone);
    cJSON_AddItemToObject(root, "parsed", parsed);

    // step 3: Operate
    int buf_len = 512;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buf, buf_len, 1);

    // step 4: Validate & Cleanup
    size_t printed_len = strlen(buf);
    char *copy = (char *)cJSON_malloc(printed_len + 1);
    memset(copy, 0, printed_len + 1);
    memcpy(copy, buf, printed_len + 1);
    cJSON_Minify(copy);
    cJSON_free(buf);
    cJSON_free(copy);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}