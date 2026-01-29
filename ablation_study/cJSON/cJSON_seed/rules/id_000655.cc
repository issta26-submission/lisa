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
//<ID> 655
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *scratch = (char *)cJSON_malloc(512);
    memset(scratch, 0, 512);
    char *raw_json = (char *)cJSON_malloc(128);
    memset(raw_json, 0, 128);
    const char *sample = " { \"a\" : 42 , \"b\" : { \"inner\" : null } , \"arr\" : [ 1 , 2 , 3 ] } ";
    memcpy(raw_json, sample, strlen(sample) + 1);
    cJSON *root = cJSON_CreateObject();
    cJSON *null_item = cJSON_CreateNull();
    cJSON *true_item = cJSON_CreateTrue();

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "nothing", null_item);
    cJSON_AddItemToObject(root, "it_true", true_item);
    cJSON_AddRawToObject(root, "raw", "{\"x\":5,\"y\":[10,20]}");
    cJSON *raw_item = cJSON_GetObjectItem(root, "raw");
    cJSON *inner_obj = cJSON_AddObjectToObject(root, "inner_obj");
    cJSON_AddNumberToObject(inner_obj, "num", 7.0);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    size_t to_copy = printed_len < 511 ? printed_len : 511;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';
    cJSON_free(printed);
    cJSON_Minify(raw_json);
    cJSON *parsed = cJSON_Parse(raw_json);
    cJSON *a_item = cJSON_GetObjectItem(parsed, "a");
    double a_val = cJSON_GetNumberValue(a_item);
    cJSON_bool has_a = cJSON_HasObjectItem(parsed, "a");
    cJSON_bool is_raw = cJSON_IsRaw(raw_item);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(raw_json);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}