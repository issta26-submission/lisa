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
//<ID> 541
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json = "{\"x\":10,\"y\":20}";
    cJSON *root = cJSON_Parse(json);
    cJSON *created_num = cJSON_CreateNumber(3.14159);
    char *printed = NULL;
    char first_char = '\0';

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(root, "added_number", 42.0);
    cJSON_AddItemToObject(root, "created_number", created_num);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    first_char = printed[0];

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}