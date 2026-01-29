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
//<ID> 652
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
    char json_buf[] = " { \"num\" : 42 , \"flag\" : true , \"child\" : { } } ";
    cJSON_Minify(json_buf);
    cJSON *parsed = cJSON_Parse(json_buf);
    cJSON *root = cJSON_CreateObject();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *null_item = cJSON_CreateNull();

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "wrapped", parsed);
    cJSON_AddItemToObject(root, "explicit_true", true_item);
    cJSON_AddItemToObject(root, "explicit_null", null_item);
    cJSON_AddItemToObject(cJSON_GetObjectItem(parsed, "child"), "raw_added", cJSON_CreateRaw("{\"x\":1,\"y\":2}"));

    // step 3: Operate / Validate
    cJSON *child_obj = cJSON_GetObjectItem(parsed, "child");
    cJSON *raw_item = cJSON_GetObjectItem(child_obj, "raw_added");
    cJSON_bool raw_flag = cJSON_IsRaw(raw_item);
    cJSON_bool has_child = cJSON_HasObjectItem(parsed, "child");
    printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    size_t to_copy = printed_len < 511 ? printed_len : 511;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}