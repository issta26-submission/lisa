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
//<ID> 1386
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"arr\":[\"one\",\"two\",\"three\"],\"flag\":false} trailing";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json, &parse_end, 0);

    // step 2: Configure
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *replacement_str = cJSON_CreateString("replaced_one");
    cJSON *replacement_true = cJSON_CreateTrue();
    cJSON_ReplaceItemInArray(arr, 0, replacement_str);
    cJSON_ReplaceItemInArray(arr, 1, replacement_true);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    const char *src = printed ? printed : "";
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, src, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);

    // step 4: Validate & Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}