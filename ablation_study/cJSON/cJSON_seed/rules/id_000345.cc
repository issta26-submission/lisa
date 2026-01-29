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
//<ID> 345
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *item1 = cJSON_CreateString("alpha");
    cJSON *item2 = cJSON_CreateString("beta");
    cJSON *item3 = cJSON_CreateString("gamma");
    cJSON *item4 = cJSON_CreateString("delta");
    cJSON *ref_item = cJSON_CreateString("epsilon");
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    char *printed = NULL;
    cJSON *retrieved = NULL;
    cJSON *dup = NULL;
    cJSON_bool is_str = 0;
    cJSON_bool same = 0;
    cJSON_bool insert_ok1 = 0;
    cJSON_bool insert_ok2 = 0;
    cJSON_bool addref_ok = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "list", array);
    cJSON_AddItemToArray(array, item1);
    insert_ok1 = cJSON_InsertItemInArray(array, 0, item2);
    addref_ok = cJSON_AddItemReferenceToArray(array, ref_item);
    insert_ok2 = cJSON_InsertItemInArray(array, 1, item4);

    // step 3: Operate / Validate
    retrieved = cJSON_GetArrayItem(array, 1);
    is_str = cJSON_IsString(retrieved);
    dup = cJSON_Duplicate(retrieved, 1);
    same = cJSON_Compare(retrieved, dup, 1);
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = (char)('0' + (int)is_str);
    buffer[1] = cJSON_GetStringValue(retrieved)[0];
    buffer[2] = (char)('0' + (int)same);
    (void)insert_ok1;
    (void)insert_ok2;
    (void)addref_ok;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}