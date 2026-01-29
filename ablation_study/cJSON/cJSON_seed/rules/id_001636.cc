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
//<ID> 1636
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *sub = NULL;
    cJSON *null_item = NULL;
    cJSON_bool has_sub = 0;
    cJSON_bool ref_ok = 0;
    cJSON_bool printed_ok = 0;
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    sub = cJSON_AddObjectToObject(root, "sub");
    null_item = cJSON_AddNullToObject(sub, "maybe");

    // step 3: Operate / Validate
    has_sub = cJSON_HasObjectItem(root, "sub");
    ref_ok = cJSON_AddItemReferenceToObject(root, "sub_alias", sub);
    printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}