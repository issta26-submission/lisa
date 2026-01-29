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
//<ID> 653
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
    cJSON *root = cJSON_CreateObject();
    cJSON *sub = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num = cJSON_CreateNumber(3.14);
    cJSON *s = cJSON_CreateString("example");
    cJSON *raw = cJSON_CreateRaw("{\"a\":1,\"b\":2}");
    cJSON *tr = cJSON_CreateTrue();
    cJSON *nu = cJSON_CreateNull();
    cJSON_bool has_sub = 0;
    cJSON_bool raw_check = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(sub, "raw", raw);
    cJSON_AddItemToObject(root, "sub", sub);
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON_AddItemToObject(root, "s", s);
    cJSON_AddItemToObject(root, "flag", tr);
    cJSON_AddItemToObject(root, "n", nu);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    size_t to_copy = printed_len < 511 ? printed_len : 511;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';
    cJSON_Minify(scratch);
    has_sub = cJSON_HasObjectItem(root, "sub");
    raw_check = cJSON_IsRaw(raw);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}