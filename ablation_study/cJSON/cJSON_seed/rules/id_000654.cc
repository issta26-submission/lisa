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
//<ID> 654
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(512);
    memset(scratch, 0, 512);
    const char *source = "  { \"k1\" : 42, \"k2\" : \"v2\" }  ";
    size_t src_len = strlen(source);
    char *buffer = (char *)cJSON_malloc(src_len + 1);
    memset(buffer, 0, src_len + 1);
    memcpy(buffer, source, src_len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *root = cJSON_CreateObject();

    // step 2: Setup / Configure
    cJSON *raw_item = cJSON_AddRawToObject(root, "raw", buffer);
    cJSON *nnull = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "nothing", nnull);
    cJSON *ttrue = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", ttrue);
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(7.0));
    cJSON_AddItemToArray(arr, cJSON_CreateString("elem"));

    // step 3: Operate / Validate
    cJSON_bool is_raw = cJSON_IsRaw(raw_item);
    cJSON_bool has_raw = cJSON_HasObjectItem(root, "raw");
    cJSON_bool combined = is_raw && has_raw;
    cJSON *combined_item = cJSON_CreateBool(combined);
    cJSON_ReplaceItemInObject(root, "flag", combined_item);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    size_t to_copy = printed_len < 511 ? printed_len : 511;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}