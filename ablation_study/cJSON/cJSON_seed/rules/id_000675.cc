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
//<ID> 675
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(128);
    memset(scratch, 0, 128);
    char *printed = NULL;
    size_t printed_len = 0;
    size_t to_copy = 0;
    cJSON *rootA = cJSON_CreateObject();
    cJSON *rootB = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("common");
    cJSON *value_item = cJSON_CreateString("value1");
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(rootA, "name", name_item);
    cJSON_AddItemToObject(rootA, "value", value_item);
    cJSON_AddStringToObject(rootB, "name", "common");
    cJSON_AddStringToObject(rootB, "value", "value1");

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(rootA);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 127 ? printed_len : 127;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';
    cJSON_free(printed);
    equal = cJSON_Compare(rootA, rootB, 1);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(rootA);
    cJSON_Delete(rootB);

    // API sequence test completed successfully
    return 66;
}