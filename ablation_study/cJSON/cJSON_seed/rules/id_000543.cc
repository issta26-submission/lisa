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
//<ID> 543
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json = "{\"number\":42}";
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON *root = cJSON_Parse(json);
    cJSON *manual_num = cJSON_CreateNumber(3.14159);
    char *printed = NULL;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(root, "added_one", 7.25);
    cJSON_AddNumberToObject(root, "added_two", 2.71828);
    cJSON_AddItemToObject(root, "manual", manual_num);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    buffer[0] = printed[0];
    buffer[1] = printed[1];
    buffer[2] = printed[2];
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}