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
//<ID> 656
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
    cJSON *raw_item = cJSON_CreateRaw("{\"x\":1,\"y\":[1,2]}");
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *null_item = cJSON_CreateNull();
    cJSON *name = cJSON_CreateString("example");
    int flags = 0;
    size_t printed_len = 0;
    size_t to_copy = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "raw", raw_item);
    cJSON_AddItemToObject(root, "flag", true_item);
    cJSON_AddItemToObject(root, "empty", null_item);
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddItemToObject(root, "sub", sub);
    cJSON_AddNumberToObject(sub, "value", 3.14);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 511 ? printed_len : 511;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';
    cJSON_free(printed);
    cJSON_Minify(scratch);
    printed_fmt = cJSON_Print(root);
    cJSON_free(printed_fmt);
    flags = (int)cJSON_IsRaw(raw_item) + (int)cJSON_HasObjectItem(root, "raw");

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}