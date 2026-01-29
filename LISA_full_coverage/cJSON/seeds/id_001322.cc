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
//<ID> 1322
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"arr\":[1,2,3],\"val\":10}";
    cJSON *parsed = cJSON_Parse(json_text);

    // step 2: Configure
    cJSON *arr = cJSON_GetObjectItemCaseSensitive(parsed, "arr");
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON *newarr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "refs", newarr);
    cJSON *newnum = cJSON_CreateNumber(0);

    // step 3: Operate
    cJSON_AddItemReferenceToArray(newarr, second);
    cJSON_InsertItemInArray(arr, 1, newnum);
    cJSON_SetNumberHelper(newnum, 99.5);
    int buf_len = 512;
    char *buffer = (char *)cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);

    // step 4: Validate & Cleanup
    size_t printed_len = strlen(buffer);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, buffer, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(buffer);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}