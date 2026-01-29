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
//<ID> 546
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json = "{\"value\":10}";
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *root = cJSON_Parse(json);
    cJSON *created_num = cJSON_CreateNumber(1.414);

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "created", created_num);
    cJSON_AddNumberToObject(root, "direct", 2.718);

    // step 3: Operate / Validate
    char *printed = cJSON_Print(root);
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