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
//<ID> 679
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    char *printed = NULL;
    size_t printed_len = 0;
    size_t to_copy = 0;
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *meta_copy = NULL;
    cJSON_bool equal = 0;
    cJSON *equal_num = NULL;

    // step 2: Setup / Configure
    cJSON_AddStringToObject(root, "title", "Test Document");
    cJSON_AddStringToObject(meta, "version", "1.0");
    cJSON_AddStringToObject(meta, "author", "tester");
    cJSON_AddItemToObject(root, "meta", meta);
    meta_copy = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToObject(root, "meta_copy", meta_copy);

    // step 3: Operate / Validate
    equal = cJSON_Compare(meta, meta_copy, 1);
    equal_num = cJSON_CreateNumber((double)equal);
    cJSON_AddItemToObject(root, "equal", equal_num);
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 255 ? printed_len : 255;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';
    cJSON_free(printed);
    printed = NULL;

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}