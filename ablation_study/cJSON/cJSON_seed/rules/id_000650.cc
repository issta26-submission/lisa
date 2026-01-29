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
//<ID> 650
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *printed_fmt = NULL;
    char *scratch = (char *)cJSON_malloc(512);
    memset(scratch, 0, 512);
    cJSON *root = cJSON_CreateObject();
    cJSON *sub = cJSON_CreateObject();
    cJSON *null_item = cJSON_CreateNull();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *str_item = cJSON_CreateString("hello world");
    cJSON *raw_added = NULL;
    cJSON_bool has_sub = 0;
    cJSON_bool is_raw_flag = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "sub", sub);
    cJSON_AddItemToObject(root, "nil", null_item);
    cJSON_AddItemToObject(root, "enabled", true_item);
    cJSON_AddItemToObject(sub, "answer", num_item);
    cJSON_AddItemToObject(sub, "greet", str_item);
    raw_added = cJSON_AddRawToObject(sub, "raw", "{\"x\":123, \"y\":[null,true,false]}");

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    size_t to_copy = printed_len < 511 ? printed_len : 511;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';
    cJSON_Minify(scratch);
    has_sub = cJSON_HasObjectItem(root, "sub");
    is_raw_flag = cJSON_IsRaw(raw_added);
    printed_fmt = cJSON_Print(root);
    size_t fmt_len = printed_fmt ? strlen(printed_fmt) : 0;
    size_t to_copy_fmt = fmt_len < 511 ? fmt_len : 511;
    memcpy(scratch, printed_fmt ? printed_fmt : "", to_copy_fmt);
    scratch[to_copy_fmt] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(printed_fmt);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}