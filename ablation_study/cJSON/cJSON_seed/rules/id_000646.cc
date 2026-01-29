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
//<ID> 646
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(512);
    memset(scratch, 0, 512);
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *s1 = cJSON_CreateString("hello");
    cJSON *s2 = cJSON_CreateString("world");

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "greet", s1);
    cJSON_AddItemToObject(child, "word", s2);
    cJSON_AddStringToObject(root, "title", "greeting");
    cJSON_AddRawToObject(root, "raw_blob", "{\"raw\":true}");

    // step 3: Operate / Validate
    char *printed = cJSON_Print(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    size_t to_copy = printed_len < 511 ? printed_len : 511;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';
    cJSON_free(printed);

    char *unprinted = cJSON_PrintUnformatted(root);
    size_t unprinted_len = unprinted ? strlen(unprinted) : 0;
    size_t space_left = 511 > to_copy ? 511 - to_copy : 0;
    size_t to_copy2 = unprinted_len < (space_left - 1) ? unprinted_len : (space_left > 1 ? space_left - 1 : 0);
    size_t offset = to_copy + 1;
    if (offset > 511) offset = 511; // harmless protective adjustment without changing control flow structure
    memcpy(scratch + offset, unprinted ? unprinted : "", to_copy2);
    scratch[offset + to_copy2] = '\0';
    cJSON_free(unprinted);

    cJSON *got = cJSON_GetObjectItem(child, "greet");
    const char *val = cJSON_GetStringValue(got);
    size_t val_len = val ? strlen(val) : 0;
    (void)val_len; // use to satisfy meaningful flow without branching

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}