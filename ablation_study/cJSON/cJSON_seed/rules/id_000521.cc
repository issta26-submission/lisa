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
//<ID> 521
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
    cJSON *s1 = cJSON_CreateString("hello");
    cJSON *added_s2 = NULL;
    char *printed = NULL;
    cJSON_bool add_ok1 = 0;
    cJSON_bool prealloc_ok = 0;

    // step 2: Setup / Configure
    add_ok1 = cJSON_AddItemToObject(root, "greeting", s1);
    added_s2 = cJSON_AddStringToObject(root, "planet", "world");

    // step 3: Operate / Validate
    prealloc_ok = cJSON_PrintPreallocated(root, buffer, 256, 1);
    printed = cJSON_Print(root);
    buffer[0] = printed[0];
    buffer[1] = (char)('0' + (char)(prealloc_ok != 0));
    buffer[2] = (char)('0' + (char)(add_ok1 != 0));
    buffer[3] = (char)('0' + (char)(added_s2 != NULL));
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}