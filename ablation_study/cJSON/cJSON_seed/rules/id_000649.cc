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
//<ID> 649
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed_fmt = NULL;
    char *printed_unfmt = NULL;
    char *scratch = (char *)cJSON_malloc(1024);
    memset(scratch, 0, 1024);
    cJSON *root = cJSON_CreateObject();
    cJSON *sub = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *s_created = cJSON_CreateString("created_string");

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "sub", sub);
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON_AddItemToArray(arr, s_created);
    cJSON *direct = cJSON_AddStringToObject(root, "direct", "direct_value");
    cJSON *inner = cJSON_AddStringToObject(sub, "inner", "inner_val");
    cJSON *raw_added = cJSON_AddRawToObject(sub, "raw", "{\"x\":1,\"y\":[1,2,3]}");

    // step 3: Operate / Validate
    printed_unfmt = cJSON_PrintUnformatted(root);
    size_t printed_unfmt_len = printed_unfmt ? strlen(printed_unfmt) : 0;
    size_t to_copy_unfmt = printed_unfmt_len < 1023 ? printed_unfmt_len : 1023;
    memcpy(scratch, printed_unfmt ? printed_unfmt : "", to_copy_unfmt);
    scratch[to_copy_unfmt] = '\0';
    cJSON_free(printed_unfmt);
    printed_fmt = cJSON_Print(root);
    size_t printed_fmt_len = printed_fmt ? strlen(printed_fmt) : 0;
    size_t to_copy_fmt = printed_fmt_len < 1023 ? printed_fmt_len : 1023;
    memcpy(scratch, printed_fmt ? printed_fmt : "", to_copy_fmt);
    scratch[to_copy_fmt] = '\0';
    cJSON_free(printed_fmt);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}