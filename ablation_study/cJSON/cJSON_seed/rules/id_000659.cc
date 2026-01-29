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
//<ID> 659
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *minified = NULL;
    char *scratch = (char *)cJSON_malloc(512);
    memset(scratch, 0, 512);
    cJSON *root = cJSON_CreateObject();
    cJSON *raw_item = cJSON_CreateRaw("{\"nested\":true, \"arr\":[1, 2, 3]}");
    cJSON *null_item = cJSON_CreateNull();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *name_item = cJSON_CreateString("example_name");
    const cJSON *lookup_raw = NULL;
    cJSON_bool has_raw = 0;
    cJSON_bool is_raw = 0;
    cJSON_bool has_flag = 0;
    size_t printed_len = 0;
    size_t to_copy = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "raw", raw_item);
    cJSON_AddItemToObject(root, "none", null_item);
    cJSON_AddItemToObject(root, "flag", true_item);
    cJSON_AddItemToObject(root, "name", name_item);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    minified = (char *)cJSON_malloc(printed_len + 1);
    memset(minified, 0, printed_len + 1);
    to_copy = printed_len;
    memcpy(minified, printed ? printed : "", to_copy);
    minified[to_copy] = '\0';
    cJSON_Minify(minified);
    has_raw = cJSON_HasObjectItem(root, "raw");
    lookup_raw = cJSON_GetObjectItem(root, "raw");
    is_raw = cJSON_IsRaw(lookup_raw);
    has_flag = cJSON_HasObjectItem(root, "flag");

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(minified);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}