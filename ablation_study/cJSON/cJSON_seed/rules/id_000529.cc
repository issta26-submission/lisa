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
//<ID> 529
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON *root = cJSON_CreateObject();
    cJSON *str1 = cJSON_CreateString("first");
    cJSON *str2 = cJSON_CreateString("second");
    char *printed = NULL;
    cJSON_bool pre_ok = 0;

    // step 2: Setup / Configure
    cJSON_AddStringToObject(root, "greeting", "hello");
    cJSON_AddItemToObject(root, "one", str1);
    cJSON_AddItemToObject(root, "two", str2);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    buffer[0] = printed[0];
    pre_ok = cJSON_PrintPreallocated(root, buffer, 256, 1);
    buffer[1] = (char)('0' + (pre_ok ? 1 : 0));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}