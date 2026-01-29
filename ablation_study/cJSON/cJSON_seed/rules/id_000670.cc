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
//<ID> 670
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
    cJSON *root1 = cJSON_CreateObject();
    cJSON *root2 = cJSON_CreateObject();
    cJSON *meta1 = cJSON_CreateObject();
    cJSON *meta2 = cJSON_CreateObject();
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root1, "meta", meta1);
    cJSON_AddItemToObject(root2, "meta", meta2);
    cJSON_AddStringToObject(root1, "id", "12345");
    cJSON_AddStringToObject(root2, "id", "12345");
    cJSON_AddStringToObject(meta1, "type", "sensor");
    cJSON_AddStringToObject(meta2, "type", "sensor");
    cJSON_AddStringToObject(meta1, "status", "active");
    cJSON_AddStringToObject(meta2, "status", "active");

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root1);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 255 ? printed_len : 255;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';
    cJSON_free(printed);
    equal = cJSON_Compare(root1, root2, 1);

    // step 4: Cleanup
    cJSON_Delete(root1);
    cJSON_Delete(root2);
    cJSON_free(scratch);

    // API sequence test completed successfully
    return 66;
}