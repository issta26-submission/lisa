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
//<ID> 657
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *formatted = NULL;
    char *minified = (char *)cJSON_malloc(512);
    memset(minified, 0, 512);
    cJSON *root = cJSON_CreateObject();
    cJSON *null_item = cJSON_CreateNull();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *str_item = cJSON_CreateString("example");
    cJSON_bool has_raw = 0;
    cJSON_bool is_raw = 0;
    cJSON *got_raw = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "null_val", null_item);
    cJSON_AddItemToObject(root, "flag", true_item);
    cJSON_AddItemToObject(root, "answer", num_item);
    cJSON_AddItemToObject(root, "name", str_item);
    cJSON_AddItemToObject(root, "raw", raw_item);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    size_t to_copy = printed_len < 511 ? printed_len : 511;
    memcpy(minified, printed ? printed : "", to_copy);
    minified[to_copy] = '\0';
    cJSON_Minify(minified);
    has_raw = cJSON_HasObjectItem(root, "raw");
    got_raw = cJSON_GetObjectItem(root, "raw");
    is_raw = cJSON_IsRaw(got_raw);
    formatted = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(formatted);
    cJSON_free(minified);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}